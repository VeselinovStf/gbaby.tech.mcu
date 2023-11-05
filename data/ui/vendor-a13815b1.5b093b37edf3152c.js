"use strict";(self.webpackChunklab_ui_v02=self.webpackChunklab_ui_v02||[]).push([["vendor-a13815b1"],{6814:(Kn,Xe,Ae)=>{Ae.d(Xe,{Do:()=>tt,EM:()=>An,HT:()=>Je,JF:()=>bn,K0:()=>V,Mx:()=>Gt,O5:()=>ze,PM:()=>_n,S$:()=>U,V_:()=>qe,Ye:()=>ve,b0:()=>be,bD:()=>Ve,ez:()=>Cn,q:()=>H,sg:()=>$e,w_:()=>Qe});var s=Ae(5879);
/**
       * @license Angular v16.2.7
       * (c) 2010-2022 Google LLC. https://angular.io/
       * License: MIT
       */let re=null;function H(){return re}function Je(e){re||(re=e)}class Qe{}const V=new s.OlP("DocumentToken");let se=(()=>{class e{historyGo(t){throw new Error("Not implemented")}static#e=this.\u0275fac=function(n){return new(n||e)};static#t=this.\u0275prov=s.Yz7({token:e,factory:function(){return(0,s.f3M)(et)},providedIn:"platform"})}return e})();const qe=new s.OlP("Location Initialized");let et=(()=>{class e extends se{constructor(){super(),this._doc=(0,s.f3M)(V),this._location=window.location,this._history=window.history}getBaseHrefFromDOM(){return H().getBaseHref(this._doc)}onPopState(t){const n=H().getGlobalEventTarget(this._doc,"window");return n.addEventListener("popstate",t,!1),()=>n.removeEventListener("popstate",t)}onHashChange(t){const n=H().getGlobalEventTarget(this._doc,"window");return n.addEventListener("hashchange",t,!1),()=>n.removeEventListener("hashchange",t)}get href(){return this._location.href}get protocol(){return this._location.protocol}get hostname(){return this._location.hostname}get port(){return this._location.port}get pathname(){return this._location.pathname}get search(){return this._location.search}get hash(){return this._location.hash}set pathname(t){this._location.pathname=t}pushState(t,n,r){this._history.pushState(t,n,r)}replaceState(t,n,r){this._history.replaceState(t,n,r)}forward(){this._history.forward()}back(){this._history.back()}historyGo(t=0){this._history.go(t)}getState(){return this._history.state}static#e=this.\u0275fac=function(n){return new(n||e)};static#t=this.\u0275prov=s.Yz7({token:e,factory:function(){return new e},providedIn:"platform"})}return e})();function oe(e,i){if(0==e.length)return i;if(0==i.length)return e;let t=0;return e.endsWith("/")&&t++,i.startsWith("/")&&t++,2==t?e+i.substring(1):1==t?e+i:e+"/"+i}function Se(e){const i=e.match(/#|\?|$/),t=i&&i.index||e.length;return e.slice(0,t-("/"===e[t-1]?1:0))+e.slice(t)}function I(e){return e&&"?"!==e[0]?"?"+e:e}let U=(()=>{class e{historyGo(t){throw new Error("Not implemented")}static#e=this.\u0275fac=function(n){return new(n||e)};static#t=this.\u0275prov=s.Yz7({token:e,factory:function(){return(0,s.f3M)(be)},providedIn:"root"})}return e})();const ye=new s.OlP("appBaseHref");let be=(()=>{class e extends U{constructor(t,n){super(),this._platformLocation=t,this._removeListenerFns=[],this._baseHref=n??this._platformLocation.getBaseHrefFromDOM()??(0,s.f3M)(V).location?.origin??""}ngOnDestroy(){for(;this._removeListenerFns.length;)this._removeListenerFns.pop()()}onPopState(t){this._removeListenerFns.push(this._platformLocation.onPopState(t),this._platformLocation.onHashChange(t))}getBaseHref(){return this._baseHref}prepareExternalUrl(t){return oe(this._baseHref,t)}path(t=!1){const n=this._platformLocation.pathname+I(this._platformLocation.search),r=this._platformLocation.hash;return r&&t?`${n}${r}`:n}pushState(t,n,r,o){const u=this.prepareExternalUrl(r+I(o));this._platformLocation.pushState(t,n,u)}replaceState(t,n,r,o){const u=this.prepareExternalUrl(r+I(o));this._platformLocation.replaceState(t,n,u)}forward(){this._platformLocation.forward()}back(){this._platformLocation.back()}getState(){return this._platformLocation.getState()}historyGo(t=0){this._platformLocation.historyGo?.(t)}static#e=this.\u0275fac=function(n){return new(n||e)(s.LFG(se),s.LFG(ye,8))};static#t=this.\u0275prov=s.Yz7({token:e,factory:e.\u0275fac,providedIn:"root"})}return e})(),tt=(()=>{class e extends U{constructor(t,n){super(),this._platformLocation=t,this._baseHref="",this._removeListenerFns=[],null!=n&&(this._baseHref=n)}ngOnDestroy(){for(;this._removeListenerFns.length;)this._removeListenerFns.pop()()}onPopState(t){this._removeListenerFns.push(this._platformLocation.onPopState(t),this._platformLocation.onHashChange(t))}getBaseHref(){return this._baseHref}path(t=!1){let n=this._platformLocation.hash;return null==n&&(n="#"),n.length>0?n.substring(1):n}prepareExternalUrl(t){const n=oe(this._baseHref,t);return n.length>0?"#"+n:n}pushState(t,n,r,o){let u=this.prepareExternalUrl(r+I(o));0==u.length&&(u=this._platformLocation.pathname),this._platformLocation.pushState(t,n,u)}replaceState(t,n,r,o){let u=this.prepareExternalUrl(r+I(o));0==u.length&&(u=this._platformLocation.pathname),this._platformLocation.replaceState(t,n,u)}forward(){this._platformLocation.forward()}back(){this._platformLocation.back()}getState(){return this._platformLocation.getState()}historyGo(t=0){this._platformLocation.historyGo?.(t)}static#e=this.\u0275fac=function(n){return new(n||e)(s.LFG(se),s.LFG(ye,8))};static#t=this.\u0275prov=s.Yz7({token:e,factory:e.\u0275fac})}return e})(),ve=(()=>{class e{constructor(t){this._subject=new s.vpe,this._urlChangeListeners=[],this._urlChangeSubscription=null,this._locationStrategy=t;const n=this._locationStrategy.getBaseHref();this._basePath=function rt(e){if(new RegExp("^(https?:)?//").test(e)){const[,t]=e.split(/\/\/[^\/]+/);return t}return e}(Se(Le(n))),this._locationStrategy.onPopState(r=>{this._subject.emit({url:this.path(!0),pop:!0,state:r.state,type:r.type})})}ngOnDestroy(){this._urlChangeSubscription?.unsubscribe(),this._urlChangeListeners=[]}path(t=!1){return this.normalize(this._locationStrategy.path(t))}getState(){return this._locationStrategy.getState()}isCurrentPathEqualTo(t,n=""){return this.path()==this.normalize(t+I(n))}normalize(t){return e.stripTrailingSlash(function it(e,i){if(!e||!i.startsWith(e))return i;const t=i.substring(e.length);return""===t||["/",";","?","#"].includes(t[0])?t:i}(this._basePath,Le(t)))}prepareExternalUrl(t){return t&&"/"!==t[0]&&(t="/"+t),this._locationStrategy.prepareExternalUrl(t)}go(t,n="",r=null){this._locationStrategy.pushState(r,"",t,n),this._notifyUrlChangeListeners(this.prepareExternalUrl(t+I(n)),r)}replaceState(t,n="",r=null){this._locationStrategy.replaceState(r,"",t,n),this._notifyUrlChangeListeners(this.prepareExternalUrl(t+I(n)),r)}forward(){this._locationStrategy.forward()}back(){this._locationStrategy.back()}historyGo(t=0){this._locationStrategy.historyGo?.(t)}onUrlChange(t){return this._urlChangeListeners.push(t),this._urlChangeSubscription||(this._urlChangeSubscription=this.subscribe(n=>{this._notifyUrlChangeListeners(n.url,n.state)})),()=>{const n=this._urlChangeListeners.indexOf(t);this._urlChangeListeners.splice(n,1),0===this._urlChangeListeners.length&&(this._urlChangeSubscription?.unsubscribe(),this._urlChangeSubscription=null)}}_notifyUrlChangeListeners(t="",n){this._urlChangeListeners.forEach(r=>r(t,n))}subscribe(t,n,r){return this._subject.subscribe({next:t,error:n,complete:r})}static#e=this.normalizeQueryParams=I;static#t=this.joinWithSlash=oe;static#n=this.stripTrailingSlash=Se;static#i=this.\u0275fac=function(n){return new(n||e)(s.LFG(U))};static#r=this.\u0275prov=s.Yz7({token:e,factory:function(){return function nt(){return new ve((0,s.LFG)(U))}()},providedIn:"root"})}return e})();function Le(e){return e.replace(/\/index.html$/,"")}function Gt(e,i){i=encodeURIComponent(i);for(const t of e.split(";")){const n=t.indexOf("="),[r,o]=-1==n?[t,""]:[t.slice(0,n),t.slice(n+1)];if(r.trim()===i)return decodeURIComponent(o)}return null}class Vt{constructor(i,t,n,r){this.$implicit=i,this.ngForOf=t,this.index=n,this.count=r}get first(){return 0===this.index}get last(){return this.index===this.count-1}get even(){return this.index%2==0}get odd(){return!this.even}}let $e=(()=>{class e{set ngForOf(t){this._ngForOf=t,this._ngForOfDirty=!0}set ngForTrackBy(t){this._trackByFn=t}get ngForTrackBy(){return this._trackByFn}constructor(t,n,r){this._viewContainer=t,this._template=n,this._differs=r,this._ngForOf=null,this._ngForOfDirty=!0,this._differ=null}set ngForTemplate(t){t&&(this._template=t)}ngDoCheck(){if(this._ngForOfDirty){this._ngForOfDirty=!1;const t=this._ngForOf;!this._differ&&t&&(this._differ=this._differs.find(t).create(this.ngForTrackBy))}if(this._differ){const t=this._differ.diff(this._ngForOf);t&&this._applyChanges(t)}}_applyChanges(t){const n=this._viewContainer;t.forEachOperation((r,o,u)=>{if(null==r.previousIndex)n.createEmbeddedView(this._template,new Vt(r.item,this._ngForOf,-1,-1),null===u?void 0:u);else if(null==u)n.remove(null===o?void 0:o);else if(null!==o){const a=n.get(o);n.move(a,u),Ue(a,r)}});for(let r=0,o=n.length;r<o;r++){const a=n.get(r).context;a.index=r,a.count=o,a.ngForOf=this._ngForOf}t.forEachIdentityChange(r=>{Ue(n.get(r.currentIndex),r)})}static ngTemplateContextGuard(t,n){return!0}static#e=this.\u0275fac=function(n){return new(n||e)(s.Y36(s.s_b),s.Y36(s.Rgc),s.Y36(s.ZZ4))};static#t=this.\u0275dir=s.lG2({type:e,selectors:[["","ngFor","","ngForOf",""]],inputs:{ngForOf:"ngForOf",ngForTrackBy:"ngForTrackBy",ngForTemplate:"ngForTemplate"},standalone:!0})}return e})();function Ue(e,i){e.context.$implicit=i.item}let ze=(()=>{class e{constructor(t,n){this._viewContainer=t,this._context=new jt,this._thenTemplateRef=null,this._elseTemplateRef=null,this._thenViewRef=null,this._elseViewRef=null,this._thenTemplateRef=n}set ngIf(t){this._context.$implicit=this._context.ngIf=t,this._updateView()}set ngIfThen(t){Ge("ngIfThen",t),this._thenTemplateRef=t,this._thenViewRef=null,this._updateView()}set ngIfElse(t){Ge("ngIfElse",t),this._elseTemplateRef=t,this._elseViewRef=null,this._updateView()}_updateView(){this._context.$implicit?this._thenViewRef||(this._viewContainer.clear(),this._elseViewRef=null,this._thenTemplateRef&&(this._thenViewRef=this._viewContainer.createEmbeddedView(this._thenTemplateRef,this._context))):this._elseViewRef||(this._viewContainer.clear(),this._thenViewRef=null,this._elseTemplateRef&&(this._elseViewRef=this._viewContainer.createEmbeddedView(this._elseTemplateRef,this._context)))}static ngTemplateContextGuard(t,n){return!0}static#e=this.\u0275fac=function(n){return new(n||e)(s.Y36(s.s_b),s.Y36(s.Rgc))};static#t=this.\u0275dir=s.lG2({type:e,selectors:[["","ngIf",""]],inputs:{ngIf:"ngIf",ngIfThen:"ngIfThen",ngIfElse:"ngIfElse"},standalone:!0})}return e})();class jt{constructor(){this.$implicit=null,this.ngIf=null}}function Ge(e,i){if(i&&!i.createEmbeddedView)throw new Error(`${e} must be a TemplateRef, but received '${(0,s.AaK)(i)}'.`)}let Cn=(()=>{class e{static#e=this.\u0275fac=function(n){return new(n||e)};static#t=this.\u0275mod=s.oAB({type:e});static#n=this.\u0275inj=s.cJS({})}return e})();const Ve="browser",En="server";function _n(e){return e===En}let An=(()=>{class e{static#e=this.\u0275prov=(0,s.Yz7)({token:e,providedIn:"root",factory:()=>new Sn((0,s.LFG)(V),window)})}return e})();class Sn{constructor(i,t){this.document=i,this.window=t,this.offset=()=>[0,0]}setOffset(i){this.offset=Array.isArray(i)?()=>i:i}getScrollPosition(){return this.supportsScrolling()?[this.window.pageXOffset,this.window.pageYOffset]:[0,0]}scrollToPosition(i){this.supportsScrolling()&&this.window.scrollTo(i[0],i[1])}scrollToAnchor(i){if(!this.supportsScrolling())return;const t=function yn(e,i){const t=e.getElementById(i)||e.getElementsByName(i)[0];if(t)return t;if("function"==typeof e.createTreeWalker&&e.body&&"function"==typeof e.body.attachShadow){const n=e.createTreeWalker(e.body,NodeFilter.SHOW_ELEMENT);let r=n.currentNode;for(;r;){const o=r.shadowRoot;if(o){const u=o.getElementById(i)||o.querySelector(`[name="${i}"]`);if(u)return u}r=n.nextNode()}}return null}(this.document,i);t&&(this.scrollToElement(t),t.focus())}setHistoryScrollRestoration(i){this.supportsScrolling()&&(this.window.history.scrollRestoration=i)}scrollToElement(i){const t=i.getBoundingClientRect(),n=t.left+this.window.pageXOffset,r=t.top+this.window.pageYOffset,o=this.offset();this.window.scrollTo(n-o[0],r-o[1])}supportsScrolling(){try{return!!this.window&&!!this.window.scrollTo&&"pageXOffset"in this.window}catch{return!1}}}class bn{}}}]);