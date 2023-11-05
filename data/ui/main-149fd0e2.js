"use strict";(self.webpackChunklab_ui_v02=self.webpackChunklab_ui_v02||[]).push([[155],{6223:(pn,ge,_)=>{_.d(ge,{EJ:()=>H,Fj:()=>E,JJ:()=>Gt,JL:()=>Bt,Kr:()=>ie,NI:()=>V,Q7:()=>U,UX:()=>fn,YN:()=>ee,_Y:()=>$t,cw:()=>v,kI:()=>Me,qu:()=>hn,sg:()=>T,u:()=>it});var r=_(5879),st=_(6814),me=_(7715),_e=_(9315),ye=_(7398);let at=(()=>{class e{constructor(t,i){this._renderer=t,this._elementRef=i,this.onChange=o=>{},this.onTouched=()=>{}}setProperty(t,i){this._renderer.setProperty(this._elementRef.nativeElement,t,i)}registerOnTouched(t){this.onTouched=t}registerOnChange(t){this.onChange=t}setDisabledState(t){this.setProperty("disabled",t)}static#t=this.\u0275fac=function(i){return new(i||e)(r.Y36(r.Qsj),r.Y36(r.SBq))};static#e=this.\u0275dir=r.lG2({type:e})}return e})(),g=(()=>{class e extends at{static#t=this.\u0275fac=function(){let t;return function(o){return(t||(t=r.n5z(e)))(o||e)}}();static#e=this.\u0275dir=r.lG2({type:e,features:[r.qOj]})}return e})();const d=new r.OlP("NgValueAccessor"),Ce={provide:d,useExisting:(0,r.Gpc)(()=>E),multi:!0},De=new r.OlP("CompositionEventMode");let E=(()=>{class e extends at{constructor(t,i,o){super(t,i),this._compositionMode=o,this._composing=!1,null==this._compositionMode&&(this._compositionMode=!function Ve(){const e=(0,st.q)()?(0,st.q)().getUserAgent():"";return/android (\d+)/.test(e.toLowerCase())}())}writeValue(t){this.setProperty("value",t??"")}_handleInput(t){(!this._compositionMode||this._compositionMode&&!this._composing)&&this.onChange(t)}_compositionStart(){this._composing=!0}_compositionEnd(t){this._composing=!1,this._compositionMode&&this.onChange(t)}static#t=this.\u0275fac=function(i){return new(i||e)(r.Y36(r.Qsj),r.Y36(r.SBq),r.Y36(De,8))};static#e=this.\u0275dir=r.lG2({type:e,selectors:[["input","formControlName","",3,"type","checkbox"],["textarea","formControlName",""],["input","formControl","",3,"type","checkbox"],["textarea","formControl",""],["input","ngModel","",3,"type","checkbox"],["textarea","ngModel",""],["","ngDefaultControl",""]],hostBindings:function(i,o){1&i&&r.NdJ("input",function(a){return o._handleInput(a.target.value)})("blur",function(){return o.onTouched()})("compositionstart",function(){return o._compositionStart()})("compositionend",function(a){return o._compositionEnd(a.target.value)})},features:[r._Bn([Ce]),r.qOj]})}return e})();function h(e){return null==e||("string"==typeof e||Array.isArray(e))&&0===e.length}function ut(e){return null!=e&&"number"==typeof e.length}const l=new r.OlP("NgValidators"),f=new r.OlP("NgAsyncValidators"),Ae=/^(?=.{1,254}$)(?=.{1,64}@)[a-zA-Z0-9!#$%&'*+/=?^_`{|}~-]+(?:\.[a-zA-Z0-9!#$%&'*+/=?^_`{|}~-]+)*@[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?(?:\.[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?)*$/;class Me{static min(n){return function dt(e){return n=>{if(h(n.value)||h(e))return null;const t=parseFloat(n.value);return!isNaN(t)&&t<e?{min:{min:e,actual:n.value}}:null}}(n)}static max(n){return function ct(e){return n=>{if(h(n.value)||h(e))return null;const t=parseFloat(n.value);return!isNaN(t)&&t>e?{max:{max:e,actual:n.value}}:null}}(n)}static required(n){return ht(n)}static requiredTrue(n){return function ft(e){return!0===e.value?null:{required:!0}}(n)}static email(n){return function pt(e){return h(e.value)||Ae.test(e.value)?null:{email:!0}}(n)}static minLength(n){return function gt(e){return n=>h(n.value)||!ut(n.value)?null:n.value.length<e?{minlength:{requiredLength:e,actualLength:n.value.length}}:null}(n)}static maxLength(n){return function mt(e){return n=>ut(n.value)&&n.value.length>e?{maxlength:{requiredLength:e,actualLength:n.value.length}}:null}(n)}static pattern(n){return function _t(e){if(!e)return w;let n,t;return"string"==typeof e?(t="","^"!==e.charAt(0)&&(t+="^"),t+=e,"$"!==e.charAt(e.length-1)&&(t+="$"),n=new RegExp(t)):(t=e.toString(),n=e),i=>{if(h(i.value))return null;const o=i.value;return n.test(o)?null:{pattern:{requiredPattern:t,actualValue:o}}}}(n)}static nullValidator(n){return null}static compose(n){return At(n)}static composeAsync(n){return Mt(n)}}function ht(e){return h(e.value)?{required:!0}:null}function w(e){return null}function yt(e){return null!=e}function vt(e){return(0,r.QGY)(e)?(0,me.D)(e):e}function Ct(e){let n={};return e.forEach(t=>{n=null!=t?{...n,...t}:n}),0===Object.keys(n).length?null:n}function Vt(e,n){return n.map(t=>t(e))}function Dt(e){return e.map(n=>function be(e){return!e.validate}(n)?n:t=>n.validate(t))}function At(e){if(!e)return null;const n=e.filter(yt);return 0==n.length?null:function(t){return Ct(Vt(t,n))}}function j(e){return null!=e?At(Dt(e)):null}function Mt(e){if(!e)return null;const n=e.filter(yt);return 0==n.length?null:function(t){const i=Vt(t,n).map(vt);return(0,_e.D)(i).pipe((0,ye.U)(Ct))}}function L(e){return null!=e?Mt(Dt(e)):null}function bt(e,n){return null===e?[n]:Array.isArray(e)?[...e,n]:[e,n]}function Et(e){return e._rawValidators}function wt(e){return e._rawAsyncValidators}function q(e){return e?Array.isArray(e)?e:[e]:[]}function F(e,n){return Array.isArray(e)?e.includes(n):e===n}function Ft(e,n){const t=q(n);return q(e).forEach(o=>{F(t,o)||t.push(o)}),t}function Ot(e,n){return q(n).filter(t=>!F(e,t))}class St{constructor(){this._rawValidators=[],this._rawAsyncValidators=[],this._onDestroyCallbacks=[]}get value(){return this.control?this.control.value:null}get valid(){return this.control?this.control.valid:null}get invalid(){return this.control?this.control.invalid:null}get pending(){return this.control?this.control.pending:null}get disabled(){return this.control?this.control.disabled:null}get enabled(){return this.control?this.control.enabled:null}get errors(){return this.control?this.control.errors:null}get pristine(){return this.control?this.control.pristine:null}get dirty(){return this.control?this.control.dirty:null}get touched(){return this.control?this.control.touched:null}get status(){return this.control?this.control.status:null}get untouched(){return this.control?this.control.untouched:null}get statusChanges(){return this.control?this.control.statusChanges:null}get valueChanges(){return this.control?this.control.valueChanges:null}get path(){return null}_setValidators(n){this._rawValidators=n||[],this._composedValidatorFn=j(this._rawValidators)}_setAsyncValidators(n){this._rawAsyncValidators=n||[],this._composedAsyncValidatorFn=L(this._rawAsyncValidators)}get validator(){return this._composedValidatorFn||null}get asyncValidator(){return this._composedAsyncValidatorFn||null}_registerOnDestroy(n){this._onDestroyCallbacks.push(n)}_invokeOnDestroyCallbacks(){this._onDestroyCallbacks.forEach(n=>n()),this._onDestroyCallbacks=[]}reset(n=void 0){this.control&&this.control.reset(n)}hasError(n,t){return!!this.control&&this.control.hasError(n,t)}getError(n,t){return this.control?this.control.getError(n,t):null}}class u extends St{get formDirective(){return null}get path(){return null}}class p extends St{constructor(){super(...arguments),this._parent=null,this.name=null,this.valueAccessor=null}}class Nt{constructor(n){this._cd=n}get isTouched(){return!!this._cd?.control?.touched}get isUntouched(){return!!this._cd?.control?.untouched}get isPristine(){return!!this._cd?.control?.pristine}get isDirty(){return!!this._cd?.control?.dirty}get isValid(){return!!this._cd?.control?.valid}get isInvalid(){return!!this._cd?.control?.invalid}get isPending(){return!!this._cd?.control?.pending}get isSubmitted(){return!!this._cd?.submitted}}let Gt=(()=>{class e extends Nt{constructor(t){super(t)}static#t=this.\u0275fac=function(i){return new(i||e)(r.Y36(p,2))};static#e=this.\u0275dir=r.lG2({type:e,selectors:[["","formControlName",""],["","ngModel",""],["","formControl",""]],hostVars:14,hostBindings:function(i,o){2&i&&r.ekj("ng-untouched",o.isUntouched)("ng-touched",o.isTouched)("ng-pristine",o.isPristine)("ng-dirty",o.isDirty)("ng-valid",o.isValid)("ng-invalid",o.isInvalid)("ng-pending",o.isPending)},features:[r.qOj]})}return e})(),Bt=(()=>{class e extends Nt{constructor(t){super(t)}static#t=this.\u0275fac=function(i){return new(i||e)(r.Y36(u,10))};static#e=this.\u0275dir=r.lG2({type:e,selectors:[["","formGroupName",""],["","formArrayName",""],["","ngModelGroup",""],["","formGroup",""],["form",3,"ngNoForm",""],["","ngForm",""]],hostVars:16,hostBindings:function(i,o){2&i&&r.ekj("ng-untouched",o.isUntouched)("ng-touched",o.isTouched)("ng-pristine",o.isPristine)("ng-dirty",o.isDirty)("ng-valid",o.isValid)("ng-invalid",o.isInvalid)("ng-pending",o.isPending)("ng-submitted",o.isSubmitted)},features:[r.qOj]})}return e})();const D="VALID",S="INVALID",y="PENDING",A="DISABLED";function $(e){return(N(e)?e.validators:e)||null}function z(e,n){return(N(n)?n.asyncValidators:e)||null}function N(e){return null!=e&&!Array.isArray(e)&&"object"==typeof e}function Pt(e,n,t){const i=e.controls;if(!(n?Object.keys(i):i).length)throw new r.vHH(1e3,"");if(!i[t])throw new r.vHH(1001,"")}function It(e,n,t){e._forEachChild((i,o)=>{if(void 0===t[o])throw new r.vHH(1002,"")})}class G{constructor(n,t){this._pendingDirty=!1,this._hasOwnPendingAsyncValidator=!1,this._pendingTouched=!1,this._onCollectionChange=()=>{},this._parent=null,this.pristine=!0,this.touched=!1,this._onDisabledChange=[],this._assignValidators(n),this._assignAsyncValidators(t)}get validator(){return this._composedValidatorFn}set validator(n){this._rawValidators=this._composedValidatorFn=n}get asyncValidator(){return this._composedAsyncValidatorFn}set asyncValidator(n){this._rawAsyncValidators=this._composedAsyncValidatorFn=n}get parent(){return this._parent}get valid(){return this.status===D}get invalid(){return this.status===S}get pending(){return this.status==y}get disabled(){return this.status===A}get enabled(){return this.status!==A}get dirty(){return!this.pristine}get untouched(){return!this.touched}get updateOn(){return this._updateOn?this._updateOn:this.parent?this.parent.updateOn:"change"}setValidators(n){this._assignValidators(n)}setAsyncValidators(n){this._assignAsyncValidators(n)}addValidators(n){this.setValidators(Ft(n,this._rawValidators))}addAsyncValidators(n){this.setAsyncValidators(Ft(n,this._rawAsyncValidators))}removeValidators(n){this.setValidators(Ot(n,this._rawValidators))}removeAsyncValidators(n){this.setAsyncValidators(Ot(n,this._rawAsyncValidators))}hasValidator(n){return F(this._rawValidators,n)}hasAsyncValidator(n){return F(this._rawAsyncValidators,n)}clearValidators(){this.validator=null}clearAsyncValidators(){this.asyncValidator=null}markAsTouched(n={}){this.touched=!0,this._parent&&!n.onlySelf&&this._parent.markAsTouched(n)}markAllAsTouched(){this.markAsTouched({onlySelf:!0}),this._forEachChild(n=>n.markAllAsTouched())}markAsUntouched(n={}){this.touched=!1,this._pendingTouched=!1,this._forEachChild(t=>{t.markAsUntouched({onlySelf:!0})}),this._parent&&!n.onlySelf&&this._parent._updateTouched(n)}markAsDirty(n={}){this.pristine=!1,this._parent&&!n.onlySelf&&this._parent.markAsDirty(n)}markAsPristine(n={}){this.pristine=!0,this._pendingDirty=!1,this._forEachChild(t=>{t.markAsPristine({onlySelf:!0})}),this._parent&&!n.onlySelf&&this._parent._updatePristine(n)}markAsPending(n={}){this.status=y,!1!==n.emitEvent&&this.statusChanges.emit(this.status),this._parent&&!n.onlySelf&&this._parent.markAsPending(n)}disable(n={}){const t=this._parentMarkedDirty(n.onlySelf);this.status=A,this.errors=null,this._forEachChild(i=>{i.disable({...n,onlySelf:!0})}),this._updateValue(),!1!==n.emitEvent&&(this.valueChanges.emit(this.value),this.statusChanges.emit(this.status)),this._updateAncestors({...n,skipPristineCheck:t}),this._onDisabledChange.forEach(i=>i(!0))}enable(n={}){const t=this._parentMarkedDirty(n.onlySelf);this.status=D,this._forEachChild(i=>{i.enable({...n,onlySelf:!0})}),this.updateValueAndValidity({onlySelf:!0,emitEvent:n.emitEvent}),this._updateAncestors({...n,skipPristineCheck:t}),this._onDisabledChange.forEach(i=>i(!1))}_updateAncestors(n){this._parent&&!n.onlySelf&&(this._parent.updateValueAndValidity(n),n.skipPristineCheck||this._parent._updatePristine(),this._parent._updateTouched())}setParent(n){this._parent=n}getRawValue(){return this.value}updateValueAndValidity(n={}){this._setInitialStatus(),this._updateValue(),this.enabled&&(this._cancelExistingSubscription(),this.errors=this._runValidator(),this.status=this._calculateStatus(),(this.status===D||this.status===y)&&this._runAsyncValidator(n.emitEvent)),!1!==n.emitEvent&&(this.valueChanges.emit(this.value),this.statusChanges.emit(this.status)),this._parent&&!n.onlySelf&&this._parent.updateValueAndValidity(n)}_updateTreeValidity(n={emitEvent:!0}){this._forEachChild(t=>t._updateTreeValidity(n)),this.updateValueAndValidity({onlySelf:!0,emitEvent:n.emitEvent})}_setInitialStatus(){this.status=this._allControlsDisabled()?A:D}_runValidator(){return this.validator?this.validator(this):null}_runAsyncValidator(n){if(this.asyncValidator){this.status=y,this._hasOwnPendingAsyncValidator=!0;const t=vt(this.asyncValidator(this));this._asyncValidationSubscription=t.subscribe(i=>{this._hasOwnPendingAsyncValidator=!1,this.setErrors(i,{emitEvent:n})})}}_cancelExistingSubscription(){this._asyncValidationSubscription&&(this._asyncValidationSubscription.unsubscribe(),this._hasOwnPendingAsyncValidator=!1)}setErrors(n,t={}){this.errors=n,this._updateControlsErrors(!1!==t.emitEvent)}get(n){let t=n;return null==t||(Array.isArray(t)||(t=t.split(".")),0===t.length)?null:t.reduce((i,o)=>i&&i._find(o),this)}getError(n,t){const i=t?this.get(t):this;return i&&i.errors?i.errors[n]:null}hasError(n,t){return!!this.getError(n,t)}get root(){let n=this;for(;n._parent;)n=n._parent;return n}_updateControlsErrors(n){this.status=this._calculateStatus(),n&&this.statusChanges.emit(this.status),this._parent&&this._parent._updateControlsErrors(n)}_initObservables(){this.valueChanges=new r.vpe,this.statusChanges=new r.vpe}_calculateStatus(){return this._allControlsDisabled()?A:this.errors?S:this._hasOwnPendingAsyncValidator||this._anyControlsHaveStatus(y)?y:this._anyControlsHaveStatus(S)?S:D}_anyControlsHaveStatus(n){return this._anyControls(t=>t.status===n)}_anyControlsDirty(){return this._anyControls(n=>n.dirty)}_anyControlsTouched(){return this._anyControls(n=>n.touched)}_updatePristine(n={}){this.pristine=!this._anyControlsDirty(),this._parent&&!n.onlySelf&&this._parent._updatePristine(n)}_updateTouched(n={}){this.touched=this._anyControlsTouched(),this._parent&&!n.onlySelf&&this._parent._updateTouched(n)}_registerOnCollectionChange(n){this._onCollectionChange=n}_setUpdateStrategy(n){N(n)&&null!=n.updateOn&&(this._updateOn=n.updateOn)}_parentMarkedDirty(n){return!n&&!(!this._parent||!this._parent.dirty)&&!this._parent._anyControlsDirty()}_find(n){return null}_assignValidators(n){this._rawValidators=Array.isArray(n)?n.slice():n,this._composedValidatorFn=function Oe(e){return Array.isArray(e)?j(e):e||null}(this._rawValidators)}_assignAsyncValidators(n){this._rawAsyncValidators=Array.isArray(n)?n.slice():n,this._composedAsyncValidatorFn=function Se(e){return Array.isArray(e)?L(e):e||null}(this._rawAsyncValidators)}}class v extends G{constructor(n,t,i){super($(t),z(i,t)),this.controls=n,this._initObservables(),this._setUpdateStrategy(t),this._setUpControls(),this.updateValueAndValidity({onlySelf:!0,emitEvent:!!this.asyncValidator})}registerControl(n,t){return this.controls[n]?this.controls[n]:(this.controls[n]=t,t.setParent(this),t._registerOnCollectionChange(this._onCollectionChange),t)}addControl(n,t,i={}){this.registerControl(n,t),this.updateValueAndValidity({emitEvent:i.emitEvent}),this._onCollectionChange()}removeControl(n,t={}){this.controls[n]&&this.controls[n]._registerOnCollectionChange(()=>{}),delete this.controls[n],this.updateValueAndValidity({emitEvent:t.emitEvent}),this._onCollectionChange()}setControl(n,t,i={}){this.controls[n]&&this.controls[n]._registerOnCollectionChange(()=>{}),delete this.controls[n],t&&this.registerControl(n,t),this.updateValueAndValidity({emitEvent:i.emitEvent}),this._onCollectionChange()}contains(n){return this.controls.hasOwnProperty(n)&&this.controls[n].enabled}setValue(n,t={}){It(this,0,n),Object.keys(n).forEach(i=>{Pt(this,!0,i),this.controls[i].setValue(n[i],{onlySelf:!0,emitEvent:t.emitEvent})}),this.updateValueAndValidity(t)}patchValue(n,t={}){null!=n&&(Object.keys(n).forEach(i=>{const o=this.controls[i];o&&o.patchValue(n[i],{onlySelf:!0,emitEvent:t.emitEvent})}),this.updateValueAndValidity(t))}reset(n={},t={}){this._forEachChild((i,o)=>{i.reset(n[o],{onlySelf:!0,emitEvent:t.emitEvent})}),this._updatePristine(t),this._updateTouched(t),this.updateValueAndValidity(t)}getRawValue(){return this._reduceChildren({},(n,t,i)=>(n[i]=t.getRawValue(),n))}_syncPendingControls(){let n=this._reduceChildren(!1,(t,i)=>!!i._syncPendingControls()||t);return n&&this.updateValueAndValidity({onlySelf:!0}),n}_forEachChild(n){Object.keys(this.controls).forEach(t=>{const i=this.controls[t];i&&n(i,t)})}_setUpControls(){this._forEachChild(n=>{n.setParent(this),n._registerOnCollectionChange(this._onCollectionChange)})}_updateValue(){this.value=this._reduceValue()}_anyControls(n){for(const[t,i]of Object.entries(this.controls))if(this.contains(t)&&n(i))return!0;return!1}_reduceValue(){return this._reduceChildren({},(t,i,o)=>((i.enabled||this.disabled)&&(t[o]=i.value),t))}_reduceChildren(n,t){let i=n;return this._forEachChild((o,s)=>{i=t(i,o,s)}),i}_allControlsDisabled(){for(const n of Object.keys(this.controls))if(this.controls[n].enabled)return!1;return Object.keys(this.controls).length>0||this.disabled}_find(n){return this.controls.hasOwnProperty(n)?this.controls[n]:null}}class kt extends v{}const C=new r.OlP("CallSetDisabledState",{providedIn:"root",factory:()=>B}),B="always";function M(e,n,t=B){J(e,n),n.valueAccessor.writeValue(e.value),(e.disabled||"always"===t)&&n.valueAccessor.setDisabledState?.(e.disabled),function Ge(e,n){n.valueAccessor.registerOnChange(t=>{e._pendingValue=t,e._pendingChange=!0,e._pendingDirty=!0,"change"===e.updateOn&&Tt(e,n)})}(e,n),function xe(e,n){const t=(i,o)=>{n.valueAccessor.writeValue(i),o&&n.viewToModelUpdate(i)};e.registerOnChange(t),n._registerOnDestroy(()=>{e._unregisterOnChange(t)})}(e,n),function Be(e,n){n.valueAccessor.registerOnTouched(()=>{e._pendingTouched=!0,"blur"===e.updateOn&&e._pendingChange&&Tt(e,n),"submit"!==e.updateOn&&e.markAsTouched()})}(e,n),function Ne(e,n){if(n.valueAccessor.setDisabledState){const t=i=>{n.valueAccessor.setDisabledState(i)};e.registerOnDisabledChange(t),n._registerOnDestroy(()=>{e._unregisterOnDisabledChange(t)})}}(e,n)}function P(e,n,t=!0){const i=()=>{};n.valueAccessor&&(n.valueAccessor.registerOnChange(i),n.valueAccessor.registerOnTouched(i)),k(e,n),e&&(n._invokeOnDestroyCallbacks(),e._registerOnCollectionChange(()=>{}))}function I(e,n){e.forEach(t=>{t.registerOnValidatorChange&&t.registerOnValidatorChange(n)})}function J(e,n){const t=Et(e);null!==n.validator?e.setValidators(bt(t,n.validator)):"function"==typeof t&&e.setValidators([t]);const i=wt(e);null!==n.asyncValidator?e.setAsyncValidators(bt(i,n.asyncValidator)):"function"==typeof i&&e.setAsyncValidators([i]);const o=()=>e.updateValueAndValidity();I(n._rawValidators,o),I(n._rawAsyncValidators,o)}function k(e,n){let t=!1;if(null!==e){if(null!==n.validator){const o=Et(e);if(Array.isArray(o)&&o.length>0){const s=o.filter(a=>a!==n.validator);s.length!==o.length&&(t=!0,e.setValidators(s))}}if(null!==n.asyncValidator){const o=wt(e);if(Array.isArray(o)&&o.length>0){const s=o.filter(a=>a!==n.asyncValidator);s.length!==o.length&&(t=!0,e.setAsyncValidators(s))}}}const i=()=>{};return I(n._rawValidators,i),I(n._rawAsyncValidators,i),t}function Tt(e,n){e._pendingDirty&&e.markAsDirty(),e.setValue(e._pendingValue,{emitModelToViewChange:!1}),n.viewToModelUpdate(e._pendingValue),e._pendingChange=!1}function Rt(e,n){const t=e.indexOf(n);t>-1&&e.splice(t,1)}function jt(e){return"object"==typeof e&&null!==e&&2===Object.keys(e).length&&"value"in e&&"disabled"in e}const V=class extends G{constructor(n=null,t,i){super($(t),z(i,t)),this.defaultValue=null,this._onChange=[],this._pendingChange=!1,this._applyFormState(n),this._setUpdateStrategy(t),this._initObservables(),this.updateValueAndValidity({onlySelf:!0,emitEvent:!!this.asyncValidator}),N(t)&&(t.nonNullable||t.initialValueIsDefault)&&(this.defaultValue=jt(n)?n.value:n)}setValue(n,t={}){this.value=this._pendingValue=n,this._onChange.length&&!1!==t.emitModelToViewChange&&this._onChange.forEach(i=>i(this.value,!1!==t.emitViewToModelChange)),this.updateValueAndValidity(t)}patchValue(n,t={}){this.setValue(n,t)}reset(n=this.defaultValue,t={}){this._applyFormState(n),this.markAsPristine(t),this.markAsUntouched(t),this.setValue(this.value,t),this._pendingChange=!1}_updateValue(){}_anyControls(n){return!1}_allControlsDisabled(){return this.disabled}registerOnChange(n){this._onChange.push(n)}_unregisterOnChange(n){Rt(this._onChange,n)}registerOnDisabledChange(n){this._onDisabledChange.push(n)}_unregisterOnDisabledChange(n){Rt(this._onDisabledChange,n)}_forEachChild(n){}_syncPendingControls(){return!("submit"!==this.updateOn||(this._pendingDirty&&this.markAsDirty(),this._pendingTouched&&this.markAsTouched(),!this._pendingChange)||(this.setValue(this._pendingValue,{onlySelf:!0,emitModelToViewChange:!1}),0))}_applyFormState(n){jt(n)?(this.value=this._pendingValue=n.value,n.disabled?this.disable({onlySelf:!0,emitEvent:!1}):this.enable({onlySelf:!0,emitEvent:!1})):this.value=this._pendingValue=n}};let $t=(()=>{class e{static#t=this.\u0275fac=function(i){return new(i||e)};static#e=this.\u0275dir=r.lG2({type:e,selectors:[["form",3,"ngNoForm","",3,"ngNativeValidate",""]],hostAttrs:["novalidate",""]})}return e})(),Jt=(()=>{class e{static#t=this.\u0275fac=function(i){return new(i||e)};static#e=this.\u0275mod=r.oAB({type:e});static#n=this.\u0275inj=r.cJS({})}return e})();const tt=new r.OlP("NgModelWithFormControlWarning"),ze={provide:u,useExisting:(0,r.Gpc)(()=>T)};let T=(()=>{class e extends u{constructor(t,i,o){super(),this.callSetDisabledState=o,this.submitted=!1,this._onCollectionChange=()=>this._updateDomValue(),this.directives=[],this.form=null,this.ngSubmit=new r.vpe,this._setValidators(t),this._setAsyncValidators(i)}ngOnChanges(t){this._checkFormPresent(),t.hasOwnProperty("form")&&(this._updateValidators(),this._updateDomValue(),this._updateRegistrations(),this._oldForm=this.form)}ngOnDestroy(){this.form&&(k(this.form,this),this.form._onCollectionChange===this._onCollectionChange&&this.form._registerOnCollectionChange(()=>{}))}get formDirective(){return this}get control(){return this.form}get path(){return[]}addControl(t){const i=this.form.get(t.path);return M(i,t,this.callSetDisabledState),i.updateValueAndValidity({emitEvent:!1}),this.directives.push(t),i}getControl(t){return this.form.get(t.path)}removeControl(t){P(t.control||null,t,!1),function Te(e,n){const t=e.indexOf(n);t>-1&&e.splice(t,1)}(this.directives,t)}addFormGroup(t){this._setUpFormContainer(t)}removeFormGroup(t){this._cleanUpFormContainer(t)}getFormGroup(t){return this.form.get(t.path)}addFormArray(t){this._setUpFormContainer(t)}removeFormArray(t){this._cleanUpFormContainer(t)}getFormArray(t){return this.form.get(t.path)}updateModel(t,i){this.form.get(t.path).setValue(i)}onSubmit(t){return this.submitted=!0,function Ut(e,n){e._syncPendingControls(),n.forEach(t=>{const i=t.control;"submit"===i.updateOn&&i._pendingChange&&(t.viewToModelUpdate(i._pendingValue),i._pendingChange=!1)})}(this.form,this.directives),this.ngSubmit.emit(t),"dialog"===t?.target?.method}onReset(){this.resetForm()}resetForm(t=void 0){this.form.reset(t),this.submitted=!1}_updateDomValue(){this.directives.forEach(t=>{const i=t.control,o=this.form.get(t.path);i!==o&&(P(i||null,t),(e=>e instanceof V)(o)&&(M(o,t,this.callSetDisabledState),t.control=o))}),this.form._updateTreeValidity({emitEvent:!1})}_setUpFormContainer(t){const i=this.form.get(t.path);(function Ht(e,n){J(e,n)})(i,t),i.updateValueAndValidity({emitEvent:!1})}_cleanUpFormContainer(t){if(this.form){const i=this.form.get(t.path);i&&function Pe(e,n){return k(e,n)}(i,t)&&i.updateValueAndValidity({emitEvent:!1})}}_updateRegistrations(){this.form._registerOnCollectionChange(this._onCollectionChange),this._oldForm&&this._oldForm._registerOnCollectionChange(()=>{})}_updateValidators(){J(this.form,this),this._oldForm&&k(this._oldForm,this)}_checkFormPresent(){}static#t=this.\u0275fac=function(i){return new(i||e)(r.Y36(l,10),r.Y36(f,10),r.Y36(C,8))};static#e=this.\u0275dir=r.lG2({type:e,selectors:[["","formGroup",""]],hostBindings:function(i,o){1&i&&r.NdJ("submit",function(a){return o.onSubmit(a)})("reset",function(){return o.onReset()})},inputs:{form:["formGroup","form"]},outputs:{ngSubmit:"ngSubmit"},exportAs:["ngForm"],features:[r._Bn([ze]),r.qOj,r.TTD]})}return e})();const Qe={provide:p,useExisting:(0,r.Gpc)(()=>it)};let it=(()=>{class e extends p{set isDisabled(t){}static#t=this._ngModelWarningSentOnce=!1;constructor(t,i,o,s,a){super(),this._ngModelWarningConfig=a,this._added=!1,this.name=null,this.update=new r.vpe,this._ngModelWarningSent=!1,this._parent=t,this._setValidators(i),this._setAsyncValidators(o),this.valueAccessor=function Z(e,n){if(!n)return null;let t,i,o;return Array.isArray(n),n.forEach(s=>{s.constructor===E?t=s:function ke(e){return Object.getPrototypeOf(e.constructor)===g}(s)?i=s:o=s}),o||i||t||null}(0,s)}ngOnChanges(t){this._added||this._setUpControl(),function Q(e,n){if(!e.hasOwnProperty("model"))return!1;const t=e.model;return!!t.isFirstChange()||!Object.is(n,t.currentValue)}(t,this.viewModel)&&(this.viewModel=this.model,this.formDirective.updateModel(this,this.model))}ngOnDestroy(){this.formDirective&&this.formDirective.removeControl(this)}viewToModelUpdate(t){this.viewModel=t,this.update.emit(t)}get path(){return function x(e,n){return[...n.path,e]}(null==this.name?this.name:this.name.toString(),this._parent)}get formDirective(){return this._parent?this._parent.formDirective:null}_checkParentType(){}_setUpControl(){this._checkParentType(),this.control=this.formDirective.addControl(this),this._added=!0}static#e=this.\u0275fac=function(i){return new(i||e)(r.Y36(u,13),r.Y36(l,10),r.Y36(f,10),r.Y36(d,10),r.Y36(tt,8))};static#n=this.\u0275dir=r.lG2({type:e,selectors:[["","formControlName",""]],inputs:{name:["formControlName","name"],isDisabled:["disabled","isDisabled"],model:["ngModel","model"]},outputs:{update:"ngModelChange"},features:[r._Bn([Qe]),r.qOj,r.TTD]})}return e})();const Ze={provide:d,useExisting:(0,r.Gpc)(()=>H),multi:!0};function te(e,n){return null==e?`${n}`:(n&&"object"==typeof n&&(n="Object"),`${e}: ${n}`.slice(0,50))}let H=(()=>{class e extends g{constructor(){super(...arguments),this._optionMap=new Map,this._idCounter=0,this._compareWith=Object.is}set compareWith(t){this._compareWith=t}writeValue(t){this.value=t;const o=te(this._getOptionId(t),t);this.setProperty("value",o)}registerOnChange(t){this.onChange=i=>{this.value=this._getOptionValue(i),t(this.value)}}_registerOption(){return(this._idCounter++).toString()}_getOptionId(t){for(const i of this._optionMap.keys())if(this._compareWith(this._optionMap.get(i),t))return i;return null}_getOptionValue(t){const i=function Xe(e){return e.split(":")[0]}(t);return this._optionMap.has(i)?this._optionMap.get(i):t}static#t=this.\u0275fac=function(){let t;return function(o){return(t||(t=r.n5z(e)))(o||e)}}();static#e=this.\u0275dir=r.lG2({type:e,selectors:[["select","formControlName","",3,"multiple",""],["select","formControl","",3,"multiple",""],["select","ngModel","",3,"multiple",""]],hostBindings:function(i,o){1&i&&r.NdJ("change",function(a){return o.onChange(a.target.value)})("blur",function(){return o.onTouched()})},inputs:{compareWith:"compareWith"},features:[r._Bn([Ze]),r.qOj]})}return e})(),ee=(()=>{class e{constructor(t,i,o){this._element=t,this._renderer=i,this._select=o,this._select&&(this.id=this._select._registerOption())}set ngValue(t){null!=this._select&&(this._select._optionMap.set(this.id,t),this._setElementValue(te(this.id,t)),this._select.writeValue(this._select.value))}set value(t){this._setElementValue(t),this._select&&this._select.writeValue(this._select.value)}_setElementValue(t){this._renderer.setProperty(this._element.nativeElement,"value",t)}ngOnDestroy(){this._select&&(this._select._optionMap.delete(this.id),this._select.writeValue(this._select.value))}static#t=this.\u0275fac=function(i){return new(i||e)(r.Y36(r.SBq),r.Y36(r.Qsj),r.Y36(H,9))};static#e=this.\u0275dir=r.lG2({type:e,selectors:[["option"]],inputs:{ngValue:"ngValue",value:"value"}})}return e})();const tn={provide:d,useExisting:(0,r.Gpc)(()=>rt),multi:!0};function ne(e,n){return null==e?`${n}`:("string"==typeof n&&(n=`'${n}'`),n&&"object"==typeof n&&(n="Object"),`${e}: ${n}`.slice(0,50))}let rt=(()=>{class e extends g{constructor(){super(...arguments),this._optionMap=new Map,this._idCounter=0,this._compareWith=Object.is}set compareWith(t){this._compareWith=t}writeValue(t){let i;if(this.value=t,Array.isArray(t)){const o=t.map(s=>this._getOptionId(s));i=(s,a)=>{s._setSelected(o.indexOf(a.toString())>-1)}}else i=(o,s)=>{o._setSelected(!1)};this._optionMap.forEach(i)}registerOnChange(t){this.onChange=i=>{const o=[],s=i.selectedOptions;if(void 0!==s){const a=s;for(let c=0;c<a.length;c++){const ot=this._getOptionValue(a[c].value);o.push(ot)}}else{const a=i.options;for(let c=0;c<a.length;c++){const R=a[c];if(R.selected){const ot=this._getOptionValue(R.value);o.push(ot)}}}this.value=o,t(o)}}_registerOption(t){const i=(this._idCounter++).toString();return this._optionMap.set(i,t),i}_getOptionId(t){for(const i of this._optionMap.keys())if(this._compareWith(this._optionMap.get(i)._value,t))return i;return null}_getOptionValue(t){const i=function en(e){return e.split(":")[0]}(t);return this._optionMap.has(i)?this._optionMap.get(i)._value:t}static#t=this.\u0275fac=function(){let t;return function(o){return(t||(t=r.n5z(e)))(o||e)}}();static#e=this.\u0275dir=r.lG2({type:e,selectors:[["select","multiple","","formControlName",""],["select","multiple","","formControl",""],["select","multiple","","ngModel",""]],hostBindings:function(i,o){1&i&&r.NdJ("change",function(a){return o.onChange(a.target)})("blur",function(){return o.onTouched()})},inputs:{compareWith:"compareWith"},features:[r._Bn([tn]),r.qOj]})}return e})(),ie=(()=>{class e{constructor(t,i,o){this._element=t,this._renderer=i,this._select=o,this._select&&(this.id=this._select._registerOption(this))}set ngValue(t){null!=this._select&&(this._value=t,this._setElementValue(ne(this.id,t)),this._select.writeValue(this._select.value))}set value(t){this._select?(this._value=t,this._setElementValue(ne(this.id,t)),this._select.writeValue(this._select.value)):this._setElementValue(t)}_setElementValue(t){this._renderer.setProperty(this._element.nativeElement,"value",t)}_setSelected(t){this._renderer.setProperty(this._element.nativeElement,"selected",t)}ngOnDestroy(){this._select&&(this._select._optionMap.delete(this.id),this._select.writeValue(this._select.value))}static#t=this.\u0275fac=function(i){return new(i||e)(r.Y36(r.SBq),r.Y36(r.Qsj),r.Y36(rt,9))};static#e=this.\u0275dir=r.lG2({type:e,selectors:[["option"]],inputs:{ngValue:"ngValue",value:"value"}})}return e})(),m=(()=>{class e{constructor(){this._validator=w}ngOnChanges(t){if(this.inputName in t){const i=this.normalizeInput(t[this.inputName].currentValue);this._enabled=this.enabled(i),this._validator=this._enabled?this.createValidator(i):w,this._onChange&&this._onChange()}}validate(t){return this._validator(t)}registerOnValidatorChange(t){this._onChange=t}enabled(t){return null!=t}static#t=this.\u0275fac=function(i){return new(i||e)};static#e=this.\u0275dir=r.lG2({type:e,features:[r.TTD]})}return e})();const on={provide:l,useExisting:(0,r.Gpc)(()=>U),multi:!0};let U=(()=>{class e extends m{constructor(){super(...arguments),this.inputName="required",this.normalizeInput=r.VuI,this.createValidator=t=>ht}enabled(t){return t}static#t=this.\u0275fac=function(){let t;return function(o){return(t||(t=r.n5z(e)))(o||e)}}();static#e=this.\u0275dir=r.lG2({type:e,selectors:[["","required","","formControlName","",3,"type","checkbox"],["","required","","formControl","",3,"type","checkbox"],["","required","","ngModel","",3,"type","checkbox"]],hostVars:1,hostBindings:function(i,o){2&i&&r.uIk("required",o._enabled?"":null)},inputs:{required:"required"},features:[r._Bn([on]),r.qOj]})}return e})(),cn=(()=>{class e{static#t=this.\u0275fac=function(i){return new(i||e)};static#e=this.\u0275mod=r.oAB({type:e});static#n=this.\u0275inj=r.cJS({imports:[Jt]})}return e})();class fe extends G{constructor(n,t,i){super($(t),z(i,t)),this.controls=n,this._initObservables(),this._setUpdateStrategy(t),this._setUpControls(),this.updateValueAndValidity({onlySelf:!0,emitEvent:!!this.asyncValidator})}at(n){return this.controls[this._adjustIndex(n)]}push(n,t={}){this.controls.push(n),this._registerControl(n),this.updateValueAndValidity({emitEvent:t.emitEvent}),this._onCollectionChange()}insert(n,t,i={}){this.controls.splice(n,0,t),this._registerControl(t),this.updateValueAndValidity({emitEvent:i.emitEvent})}removeAt(n,t={}){let i=this._adjustIndex(n);i<0&&(i=0),this.controls[i]&&this.controls[i]._registerOnCollectionChange(()=>{}),this.controls.splice(i,1),this.updateValueAndValidity({emitEvent:t.emitEvent})}setControl(n,t,i={}){let o=this._adjustIndex(n);o<0&&(o=0),this.controls[o]&&this.controls[o]._registerOnCollectionChange(()=>{}),this.controls.splice(o,1),t&&(this.controls.splice(o,0,t),this._registerControl(t)),this.updateValueAndValidity({emitEvent:i.emitEvent}),this._onCollectionChange()}get length(){return this.controls.length}setValue(n,t={}){It(this,0,n),n.forEach((i,o)=>{Pt(this,!1,o),this.at(o).setValue(i,{onlySelf:!0,emitEvent:t.emitEvent})}),this.updateValueAndValidity(t)}patchValue(n,t={}){null!=n&&(n.forEach((i,o)=>{this.at(o)&&this.at(o).patchValue(i,{onlySelf:!0,emitEvent:t.emitEvent})}),this.updateValueAndValidity(t))}reset(n=[],t={}){this._forEachChild((i,o)=>{i.reset(n[o],{onlySelf:!0,emitEvent:t.emitEvent})}),this._updatePristine(t),this._updateTouched(t),this.updateValueAndValidity(t)}getRawValue(){return this.controls.map(n=>n.getRawValue())}clear(n={}){this.controls.length<1||(this._forEachChild(t=>t._registerOnCollectionChange(()=>{})),this.controls.splice(0),this.updateValueAndValidity({emitEvent:n.emitEvent}))}_adjustIndex(n){return n<0?n+this.length:n}_syncPendingControls(){let n=this.controls.reduce((t,i)=>!!i._syncPendingControls()||t,!1);return n&&this.updateValueAndValidity({onlySelf:!0}),n}_forEachChild(n){this.controls.forEach((t,i)=>{n(t,i)})}_updateValue(){this.value=this.controls.filter(n=>n.enabled||this.disabled).map(n=>n.value)}_anyControls(n){return this.controls.some(t=>t.enabled&&n(t))}_setUpControls(){this._forEachChild(n=>this._registerControl(n))}_allControlsDisabled(){for(const n of this.controls)if(n.enabled)return!1;return this.controls.length>0||this.disabled}_registerControl(n){n.setParent(this),n._registerOnCollectionChange(this._onCollectionChange)}_find(n){return this.at(n)??null}}function pe(e){return!!e&&(void 0!==e.asyncValidators||void 0!==e.validators||void 0!==e.updateOn)}let hn=(()=>{class e{constructor(){this.useNonNullable=!1}get nonNullable(){const t=new e;return t.useNonNullable=!0,t}group(t,i=null){const o=this._reduceControls(t);let s={};return pe(i)?s=i:null!==i&&(s.validators=i.validator,s.asyncValidators=i.asyncValidator),new v(o,s)}record(t,i=null){const o=this._reduceControls(t);return new kt(o,i)}control(t,i,o){let s={};return this.useNonNullable?(pe(i)?s=i:(s.validators=i,s.asyncValidators=o),new V(t,{...s,nonNullable:!0})):new V(t,i,o)}array(t,i,o){const s=t.map(a=>this._createControl(a));return new fe(s,i,o)}_reduceControls(t){const i={};return Object.keys(t).forEach(o=>{i[o]=this._createControl(t[o])}),i}_createControl(t){return t instanceof V||t instanceof G?t:Array.isArray(t)?this.control(t[0],t.length>1?t[1]:null,t.length>2?t[2]:null):this.control(t)}static#t=this.\u0275fac=function(i){return new(i||e)};static#e=this.\u0275prov=r.Yz7({token:e,factory:e.\u0275fac,providedIn:"root"})}return e})(),fn=(()=>{class e{static withConfig(t){return{ngModule:e,providers:[{provide:tt,useValue:t.warnOnNgModelWithFormControl??"always"},{provide:C,useValue:t.callSetDisabledState??B}]}}static#t=this.\u0275fac=function(i){return new(i||e)};static#e=this.\u0275mod=r.oAB({type:e});static#n=this.\u0275inj=r.cJS({imports:[cn]})}return e})()}}]);